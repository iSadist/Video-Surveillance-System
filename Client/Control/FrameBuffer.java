package Control;

import java.util.LinkedList;
import java.util.ListIterator;

public class FrameBuffer {

	private LinkedList<ServerFrame> frames;
	
	public FrameBuffer() {
		frames = new LinkedList<ServerFrame>();
	}
	
	/**
	 * Removes and returns the oldest ServerFrame from this buffer.
	 * 
	 * @return Oldest ServerFrame in the buffer.
	 */
	public ServerFrame pop() {
		return frames.removeLast();
	}
	
	/**
	 * Retrieves, but does not remove, the oldest ServerFrame in this buffer,
	 * or returns null if this buffer is empty.
	 * 
	 * @return oldest ServerFrame, or null if this buffer is empty.
	 */
	public ServerFrame peek() {
		return frames.peekLast();
	}
	
	/**
	 * Adds the frame to the buffer.
	 * 
	 * @param frame	ServerFrame to add.
	 */
	public void add(ServerFrame frame) {
		int insertionIndex = 0;
		boolean insertionPointFound = false;
		ListIterator<ServerFrame> it = frames.listIterator();
		
		while (it.hasNext() && !insertionPointFound) {
			insertionIndex = it.nextIndex();
			ServerFrame currentNode = it.next();
			
			if ((null != currentNode) &&
				(frame.getSentTime().after(currentNode.getSentTime()))) {
				insertionPointFound = true;
			}
		}
		
		if (insertionPointFound){
			frames.add(insertionIndex, frame);
		} else {
			frames.addLast(frame);
		}
	}
	
	/**
	 * Returns the number of elements in this list.
	 * 
	 * @return the number of elements in this list.
	 */
	public int size() {
		return frames.size();
	}
	
}
